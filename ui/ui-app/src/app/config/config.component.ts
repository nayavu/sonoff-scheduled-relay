import { Component, OnInit, ViewChild } from '@angular/core';
import { HttpClient } from "@angular/common/http";
import { environment } from "../../environments/environment";
import { TimePipe } from "../devices/time.pipe";
import { NgForm } from "@angular/forms";

@Component({
  selector: 'app-config',
  templateUrl: './config.component.html'
})
export class ConfigComponent implements OnInit {
  public config: Config;
  public status = '';
  public msg = '';

  @ViewChild("configForm", {static: false}) configForm: NgForm;

  constructor(private http: HttpClient) { }

  ngOnInit() {
    this.http.get(environment.backend + '/config').subscribe(
        (data: Config) => {
          this.config = data;
        },
        error => {

          if (error.status == 404) {
            this.msg = 'No persisted configuration found';
          } else if (error.status == 500) {
            this.msg = 'Configuration file corrupted';
          } else {
            this.msg = 'Unknown error with configuration';
          }

          this.config = new Config();
        })
  }

  save() {
    this.msg = '';

    const utcOffset = TimePipe.timeToNumber(this.configForm.control.get("utcOffset").value);
    console.log(utcOffset);
    if (utcOffset !== NaN) {
        this.config.utcOffset = utcOffset;
    }

    this.status = 'saving';
    this.http.post(environment.backend + '/config', this.config).subscribe(
        data => {
          this.status = 'saved';
        },
        error => {
          this.status = 'error';
        })
  }

  reboot(configMode = false) {
    this.http.post(environment.backend + '/reboot' + (configMode ? '?config=true' : ''), null).subscribe(
        data => {
          this.status = 'rebooting';
        },
        error => {
          this.status = 'error';
        })
  }
}

export class Config {
  deviceName: string = 'device';
  wifiSsid: string = '';
  wifiPass: string = '';
  ntpHost: string = '';
  utcOffset: number;
}
