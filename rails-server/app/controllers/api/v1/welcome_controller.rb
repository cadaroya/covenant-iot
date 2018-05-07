module Api
    module V1
        class WelcomeController < ApplicationController
            def index
                @reading = {
                    "ip_address": "120.0.0.7"
                    "threshold": 1
                    "analog_reading": 123
                }

                render json: {status: 'SUCCESS', data: @reading}
                    
            end
        end
    end
end
