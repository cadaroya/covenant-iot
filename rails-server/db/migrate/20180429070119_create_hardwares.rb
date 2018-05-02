class CreateHardwares < ActiveRecord::Migration[5.1]
  def change
    create_table :hardwares do |t|
      t.integer :ipv4_octet1
      t.integer :ipv4_octet2
      t.integer :ipv4_octet3
      t.integer :ipv4_octet4
      t.integer :mac_addr_byte1
      t.integer :mac_addr_byte2
      t.integer :mac_addr_byte3
      t.integer :mac_addr_byte4
      t.integer :mac_addr_byte5
      t.integer :mac_addr_byte6
      t.integer :token
      t.timestamps
    end
  end
end
