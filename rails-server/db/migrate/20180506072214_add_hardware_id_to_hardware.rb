class AddHardwareIdToHardware < ActiveRecord::Migration[5.1]
  def change
  	add_column :hardwares, :hardware_id, :integer
  	add_index :hardwares, :hardware_id
  	add_index :hardwares, [:ipv4_octet1, :ipv4_octet2, :ipv4_octet3, :ipv4_octet4], :name => 'ip_addr_index'
  	add_index :readings, :token
  	add_index :readings, :hardware_id
  end
end
