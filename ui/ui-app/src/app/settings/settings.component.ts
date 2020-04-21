import { Component, OnInit } from '@angular/core';
import { HttpClient } from "@angular/common/http";
import { environment } from "../../environments/environment";
import { FormBuilder, FormGroup, Validators } from "@angular/forms";
import { NotificationService, NotificationServiceHelper } from "../common/notification/notification.service";

@Component({
  selector: 'app-settings',
  templateUrl: './settings.component.html'
})
export class SettingsComponent implements OnInit {
  public form: FormGroup;

  public loading = false;
  public saving = false;

  constructor(private http: HttpClient, private formBuilder: FormBuilder, private notificationService: NotificationService) { }

  ngOnInit() {
    this.form = this.formBuilder.group({
      deviceName: [null, [Validators.required, Validators.maxLength(32)]],
      wifiSsid: [null, [Validators.required, Validators.maxLength(32)]],
      wifiPass: [null, [Validators.maxLength(64)]],
      ntpHost: [null, [Validators.maxLength(99)]],
      utcOffset: [0, [Validators.required]]
    });

    this.loading = true;
    this.http.get(environment.backend + '/settings').subscribe(
      (data: any) => {
        this.loading = false;
        this.form.setValue(data);
        this.notificationService.reset();
      },
      error => {
        this.loading = false;
        this.notificationService.notify(NotificationServiceHelper.deriveHttpErrorMessageForLoadAction('load settings', error));
      }
    );
  }

  onSave() {
    if (this.form.invalid) {
      Object.keys(this.form.controls)
        .forEach(controlName => this.form.controls[controlName].markAsTouched());
      return;
    }

    this.saving = true;
    this.http.post(environment.backend + '/settings', this.form.value).subscribe(
      data => {
        this.saving = false;
        this.notificationService.info('Saved');
      },
      error => {
        this.saving = false;
        this.notificationService.notify(NotificationServiceHelper.deriveHttpErrorMessage('save settings', error));
        console.log(error);
      });
  }

  reboot(configMode = false) {
    this.http.post(environment.backend + '/reboot' + (configMode ? '?config=true' : ''), null).subscribe(
      data => {
        this.notificationService.info(configMode ? 'Rebooting to config mode' : 'Rebooting');
      },
      error => {
        this.saving = false;
        this.notificationService.notify(NotificationServiceHelper.deriveHttpErrorMessage('reboot', error));
        console.log(error);
      })
  }
}
