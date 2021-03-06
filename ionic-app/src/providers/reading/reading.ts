import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';

/*
  Generated class for the WeatherProvider provider.

  See https://angular.io/guide/dependency-injection for more info on providers
  and Angular DI.
*/
@Injectable()
export class ReadingProvider {

  //apiKey = 'e0f0aed7a09e5948';
  url;

  constructor(public http: HttpClient) {
    this.url = 'http://immense-plateau-44759.herokuapp.com/api/v1/';
    //+this.apiKey+'/conditions/q';
  }

  getReading(){
    var response = this.http.get(this.url+'reading');
    return response;
  }

  getReadingFromID(hardware_id, token){
    var response = this.http.get(this.url+hardware_id+"/"+token);
    return response;
  }
}
