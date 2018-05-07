import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { ReadingProvider} from '../../providers/reading/reading';
import { Storage } from '@ionic/storage';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {
  data: any;
  hardware_id: number;
  token: number;

  constructor(public navCtrl: NavController,
    private readingProvider:ReadingProvider,
    private storage: Storage) {
  }

  ionViewWillEnter(){
    this.storage.get('hardware_data').then((data) => {
      if (data != null){
        let data_str = JSON.parse(data);
        this.hardware_id = parseInt(data_str.hardware_id);
        this.token = parseInt(data_str.token);
        this.getReadingFromID(this.hardware_id, this.token);
      }
      else{
        this.hardware_id = 0;
        this.token = 0;
      }
    });
  }

  getReading(){
    this.readingProvider.getReading().subscribe((data) => {
      console.log("Home.ts get Reading");
      this.data = data["data"];
    });
  }

  getReadingFromID(hardware_id, token){
    this.readingProvider.getReadingFromID(hardware_id, token).subscribe((data) => {
      console.log("Home.ts get Reading from ID");
      console.log("Hardware ID: "+hardware_id);
      console.log("Token: "+ token);
      this.data = data["data"];
      console.log(this.data);
    });
  }

}
