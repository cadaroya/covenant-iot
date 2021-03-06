# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20180506072214) do

  create_table "hardwares", force: :cascade do |t|
    t.integer "ipv4_octet1"
    t.integer "ipv4_octet2"
    t.integer "ipv4_octet3"
    t.integer "ipv4_octet4"
    t.integer "mac_addr_byte1"
    t.integer "mac_addr_byte2"
    t.integer "mac_addr_byte3"
    t.integer "mac_addr_byte4"
    t.integer "mac_addr_byte5"
    t.integer "mac_addr_byte6"
    t.integer "token"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
    t.integer "hardware_id"
    t.index ["hardware_id"], name: "index_hardwares_on_hardware_id"
    t.index ["ipv4_octet1", "ipv4_octet2", "ipv4_octet3", "ipv4_octet4"], name: "ip_addr_index"
  end

  create_table "readings", force: :cascade do |t|
    t.integer "hardware_id"
    t.integer "token"
    t.integer "analog_reading"
    t.integer "threshold"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
    t.index ["hardware_id"], name: "index_readings_on_hardware_id"
    t.index ["token"], name: "index_readings_on_token"
  end

end
