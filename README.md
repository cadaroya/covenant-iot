# Basic Image Upload Server in Golang + DB + Auth

## Description

The application features a basic image upload functionality in Go. 

## Key Notes about Features
1. MVC architectural pattern. Just to add some organization.
2. Uses http/template to render the view which is `index.gtpl`
3. The sqlite3 with Gorm was used to interact with the local database (test.db)
4. "sharedKey" is saved in the .env file and loaded via godotenv
5. Basic authoriziation through the checking of shared secret key (No HMAC or ECDSA).

Others: 
- Env variables were included in gitignore for "security purposes" :)
- tmp folder storing the images were also ignored

## Basic Setup:

1. My working directory was C:\Users\Carlos\go\src\github.com\cadaroya\brankas\app
2. Install the packages:
"github.com/jinzhu/gorm"
"github.com/gorilla/mux"
"github.com/jinzhu/gorm/dialects/sqlite"
"github.com/joho/godotenv"

3. Navigate to the \app folder then type "go run main.go"

4. Handler function is at localhost:8080/upload

