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
  threshold: number;
  interval: number;

  constructor(public navCtrl: NavController,
    public navParams: NavParams,
    private storage: Storage) {
      this.storage.get('settings').then((data) => {
        if (data != null){
          let data_str = JSON.parse(data);
          this.hardware_id = parseInt(data_str.hardware_id);
          this.token = parseInt(data_str.token);
          this.threshold = parseInt(data_str.threshold);
          this.interval = parseInt(data_str.interval);
        }
        else{
          this.hardware_id = 0;
          this.token = 0;
          this.threshold = 145;
          this.interval = 10;
        }
      });
  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad SettingsPage');
  }

  saveForm(){
    let settings = {
      hardware_id: this.hardware_id,
      token: this.token,
      threshold: this.threshold,
      interval: this.interval
    };
    this.storage.set('settings', JSON.stringify(settings));
    console.log(settings);
  }
}
