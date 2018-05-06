module Api
    module V1
        require 'json'
        class ReadingController < ApplicationController
            def index
                ip = request.remote_ip.split(".")
                requester = Hardware.where('ipv4_octet1 = ? AND ipv4_octet2 = ? AND ipv4_octet3 = ? AND ipv4_octet4 = ?', 
                                            ip[0].to_i, ip[1].to_i, ip[2].to_i, ip[3].to_i).last
                if requester.nil?
                    @reading = {
                    "ip_address": [ip[0].to_i, ip[1].to_i, ip[2].to_i, ip[3].to_i], #shet gumagana to
                    "threshold": nil,
                    "analog_reading": nil
                }
                else
                    recent_data = Reading.where('token = ? AND hardware_id = ?', requester.token, requester.hardware_id).order('created_at').last
                    @reading = {
                        "ip_address": [ip[0].to_i, ip[1].to_i, ip[2].to_i, ip[3].to_i], #shet gumagana to
                        "threshold": recent_data.threshold,
                        "analog_reading": recent_data.analog_reading
                    }
                end
                render json: {status: 'SUCCESS', data: @reading}

            end

            def create
                ip_address = request.remote_ip.split(".")
                hardware = Hardware.new()
                hardware.ipv4_octet1 = ip_address[0].to_i
                hardware.ipv4_octet2 = ip_address[1].to_i
                hardware.ipv4_octet3 = ip_address[2].to_i
                hardware.ipv4_octet4 = ip_address[3].to_i
                request_raw = request.raw_post
                payload = JSON.parse(request_raw)
                #render string
                hardware.token = payload["token"]
                hardware.hardware_id = payload["hardware_id"]
                hardware.save()
                reading = Reading.new()

                reading.hardware_id = payload["hardware_id"];
                reading.analog_reading = payload["analog_reading"];
                reading.token = payload["token"];
                reading.threshold = payload["threshold"];
                reading.save()
                render json: {status: 'SUCCESS', data: payload}


            end

            def show

            end
        end 
    end
end