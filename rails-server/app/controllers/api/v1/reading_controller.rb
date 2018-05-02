module Api
    module V1
        class ReadingController < ApplicationController
            def index

                @reading = {
                    "id": 1,
                    "data": "Hello from the webserver! ReadingController"
                };
                render json: {status: 'SUCCESS', message:'Loaded articles', data: @reading}
            end
        end 
    end
end