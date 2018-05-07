import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams } from 'ionic-angular';
import { Storage } from '@ionic/storage';
import { HomePage } from '../home/home';

@IonicPage()
@Component({
  selector: 'page-settings',
  templateUrl: 'settings.html',
})
export class SettingsPage {

  hardware_id: number;
  token: number;

  constructor(public navCtrl: NavController,
    public navParams: NavParams,
    private storage: Storage) {
      this.storage.get('hardware_data').then((data) => {
        if (data != null){
          let data_str = JSON.parse(data);
          this.hardware_id = parseInt(data_str.hardware_id);
          this.token = parseInt(data_str.token);
        }
        else{
          this.hardware_id = 0;
          this.token = 0;
        }
      });
  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad SettingsPage');
  }

  saveForm(){
    let hardware_data = {
      hardware_id: this.hardware_id,
      token: this.token
    };
    this.storage.set('hardware_data', JSON.stringify(hardware_data));
    console.log(hardware_data);
  }
}
