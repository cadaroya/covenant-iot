import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { ReadingProvider} from '../../providers/reading/reading';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {
  data: any;
  hardware_id: any;
  location: {
    city:string,
    state:string
  }
  constructor(public navCtrl: NavController, private readingProvider:ReadingProvider) {

  }

  ionViewWillEnter(){
    this.location = {
      city: 'Miami',
      state: 'FL'
    }

    this.readingProvider.getReading().subscribe((data) => console.log(data));
  }

  getReading(){
    this.readingProvider.getReading().subscribe((data) => {
      console.log("Home.ts");
      this.data = data;
    });
  }

  getID(){
    this.readingProvider.getReading().subscribe((data) => {
      console.log("Home.ts");
      this.data = data.data;
    });
  }

}
