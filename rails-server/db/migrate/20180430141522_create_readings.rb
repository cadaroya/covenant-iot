class CreateReadings < ActiveRecord::Migration[5.1]
  def change
    create_table :readings do |t|
      t.integer :hardware_id
      t.integer :token
      t.integer :analog_reading
      t.integer :threshold

      t.timestamps
    end
  end
end
