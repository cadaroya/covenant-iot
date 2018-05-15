import { BackgroundMode } from '@ionic-native/background-mode';
import { Component } from '@angular/core';
import { NavController, Events } from 'ionic-angular';
import { ReadingProvider} from '../../providers/reading/reading';
import { Storage } from '@ionic/storage';
import { LocalNotifications } from '@ionic-native/local-notifications';

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
    public events: Events,
    private backgroundMode: BackgroundMode,
    private localNotifications: LocalNotifications) {
      this.backgroundMode.enable();
      this.backgroundMode.excludeFromTaskList();
      this.events.subscribe('getTimer', (time, global_time) =>{
        this.time = time;
        if (time % (this.interval - 1) == 0){
          this.getReadingFromID(this.hardware_id, this.token);
        }
        if (global_time % 20 == 0){
          if (this.data["threshold"] == "Yellow"){
            this.localNotifications.schedule({
              id: 1,
              text: 'Threshold is yellow.',
            });
          }
          if (this.data["threshold"] == "Orange"){
            this.localNotifications.schedule({
              id: 1,
              text: 'Threshold is orange.',
            });
          }
          if (this.data["threshold"] == "Red"){
            this.localNotifications.schedule({
              id: 1,
              text: 'Threshold is red.',
            });
          }
        }
      })
  }

  ionViewWillEnter(){
    this.storage.get('settings').then((data) => {
      if (data != null){
        let data_str = JSON.parse(data);
        this.hardware_id = parseInt(data_str.hardware_id);
        this.token = parseInt(data_str.token);
        this.interval = parseInt(data_str.interval);
        this.getReadingFromID(this.hardware_id, this.token);
      }
      else{
        this.hardware_id = 12345;
        this.token = 7397;
        this.interval = 10;
      }
    });
  }

  getReadingFromID(hardware_id, token){
    this.events.publish('resetTimer');
    this.readingProvider.getReadingFromID(hardware_id, token).subscribe((data) => {
      this.data = data["data"];
      if (this.data["threshold"] == 0){
        this.data["threshold"] = "Green";
      }
      else if (this.data["threshold"] == 1){
        this.data["threshold"] = "Yellow";
      }
      else if (this.data["threshold"] == 2){
        this.data["threshold"] = "Orange";
      }
      else if (this.data["threshold"] == 3){
        this.data["threshold"] = "Red";
      }
      else {
        this.data["threshold"] = "undefined";
      }
    });
  }
}
