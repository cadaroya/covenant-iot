module Api
    module V1
        class WelcomeController < ApplicationController
            def index
                render "index"
            end
        end
    end
end
