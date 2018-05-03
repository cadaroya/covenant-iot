module Api
    module V1
        require 'json'
        class ReadingController < ApplicationController
            def index
                ip = request.ip().split(".")
                @reading = {
                    "id":1,
                    "data": [ip[0].to_i, ip[1].to_i, ip[2].to_i, ip[3].to_i] #shet gumagana to
                }
                render json: {status: 'SUCCESS', is_localhost:request.local?(), data: @reading}
            end

            def create
                ip_address = request.remote_ip.split(".")
                hardware = Hardware.new()
                hardware.ipv4_octet1 = ip_address[0].to_i
                hardware.ipv4_octet2 = ip_address[1].to_i
                hardware.ipv4_octet3 = ip_address[2].to_i
                hardware.ipv4_octet4 = ip_address[3].to_i
                hardware.save()
                #string = request.raw_post
                #json = JSON.parse(string)

                #render json: {status: 'SUCCESS', data: request.raw_post}
                #reading = Reading.new()

                #reading.analog_reading = data['analog_reading']
                #reading.save()
                #render data


            end

            def show

            end
        end 
    end
end