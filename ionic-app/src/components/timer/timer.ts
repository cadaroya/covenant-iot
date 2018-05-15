import { Component } from '@angular/core';
import { Events } from 'ionic-angular';

/**
 * Generated class for the TimerComponent component.
 *
 * See https://angular.io/api/core/Component for more info on Angular
 * Components.
 */
@Component({
  selector: 'timer',
  templateUrl: 'timer.html'
})
export class TimerComponent {

  time: number;
  global_time: number;


  constructor(public events: Events) {
    
    this.time = 0;
    this.global_time = 0;
    
    setInterval(() => { 
      this.countdown(); // Now the "this" still references the component
    }, 1000);

    this.events.subscribe('resetTimer', ()=>{
      this.time = 0;
    });
  }

  countdown(){
    this.time += 1;
    this.global_time += 1;
    this.events.publish('getTimer', this.time, this.global_time);
  }

}
