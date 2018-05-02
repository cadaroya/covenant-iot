import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { ReadingProvider} from '../../providers/reading/reading';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {
  weather: any;
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

}
