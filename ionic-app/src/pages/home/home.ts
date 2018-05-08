import { Component } from '@angular/core';
import { NavController, Events } from 'ionic-angular';
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
  threshold: number;
  interval: number;
  time: number;

  constructor(public navCtrl: NavController,
    private readingProvider:ReadingProvider,
    private storage: Storage,
    public events: Events) {
      this.events.subscribe('getTimer', (data) =>{
        this.time = data;
        if (data % (this.interval - 1) == 0){
          this.getReadingFromID(this.hardware_id, this.token);
        }
      })
  }

  ionViewWillEnter(){
    this.storage.get('settings').then((data) => {
      if (data != null){
        let data_str = JSON.parse(data);
        this.hardware_id = parseInt(data_str.hardware_id);
        this.token = parseInt(data_str.token);
        this.threshold = parseInt(data_str.threshold);
        this.interval = parseInt(data_str.interval);
        this.getReadingFromID(this.hardware_id, this.token);
      }
      else{
        this.hardware_id = 12345;
        this.token = 7397;
        this.threshold = 145;
        this.interval = 10;
      }
    });
  }

  getReading(){
    this.readingProvider.getReading().subscribe((data) => {
      this.data = data["data"];
    });
  }

  getReadingFromID(hardware_id, token){
    this.events.publish('resetTimer');
    this.readingProvider.getReadingFromID(hardware_id, token).subscribe((data) => {
      this.data = data["data"];
      console.log(this.data);
    });
  }
}
