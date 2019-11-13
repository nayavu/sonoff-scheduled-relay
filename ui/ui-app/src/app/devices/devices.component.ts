import { Component, OnInit } from '@angular/core';
import { HttpClient } from "@angular/common/http";
import { environment } from "../../environments/environment";
import { TimePipe } from "./time.pipe";

@Component({
  selector: 'app-devices',
  templateUrl: './devices.component.html'
})
export class DevicesComponent implements OnInit {

  public config: ScheduledRelayConfig;
  public state: ScheduledRelayState;

  public status = '';

  constructor(private http: HttpClient) { }

  ngOnInit() {
    this.http.get(environment.backend + '/relay/config').subscribe(
        (data: ScheduledRelayConfig) => {
          this.config = data;
        },
        error => {
          this.config = new ScheduledRelayConfig();
        });
    this.loadState();
  }

  private loadState() {
    this.http.get(environment.backend + '/relay/state').subscribe((data: ScheduledRelayState) => {
      this.state = data;
    });
  }

  timeToNumber(value: string): number {
    return TimePipe.timeToNumber(value);
  }

  save() {
    this.status = 'saving';
    this.http.post(environment.backend + '/relay/config', this.config).subscribe(
        data => {
          this.status = 'saved';
          this.loadState();
        },
        error => {
          this.status = 'error';
        });
  }
}

export class ScheduledRelayConfig {
  startTime: number = 0;
  stopTime: number = 0;
}
export class ScheduledRelayState {
  on: boolean = false;
  time: string = '';
}
