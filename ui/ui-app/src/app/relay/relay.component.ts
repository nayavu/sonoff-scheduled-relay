import { Component, OnInit } from '@angular/core';
import { HttpClient } from "@angular/common/http";
import { environment } from "../../environments/environment";
import { FormBuilder, FormGroup, Validators } from "@angular/forms";
import { NotificationService, NotificationServiceHelper } from "../common/notification/notification.service";

@Component({
  selector: 'app-devices',
  templateUrl: './relay.component.html',
  styles: [
    '.power-on { color: #009900; }',
    '.power-off { color: #FF0000; }'
  ]
})
export class RelayComponent implements OnInit {
  public form: FormGroup;

  public loading = true;
  public saving = false;

  public state: ScheduledRelayState;

  constructor(private http: HttpClient, private formBuilder: FormBuilder, private notificationService: NotificationService) { }

  ngOnInit() {
    this.form = this.formBuilder.group({
      startTime: [0, [Validators.required]],
      stopTime: [0, [Validators.required]]
    });

    this.loadSettings();
    this.loadState();
  }

  private loadSettings() {
    this.http.get(environment.backend + '/relay/settings').subscribe(
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

  private loadState() {
    this.http.get(environment.backend + '/relay/state').subscribe(
      (data: ScheduledRelayState) => {
        this.state = data;
      },
      error => {
        this.notificationService.notify(NotificationServiceHelper.deriveHttpErrorMessageForLoadAction('load state', error));
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
    this.http.post(environment.backend + '/relay/settings', this.form.value).subscribe(
      data => {
        this.saving = false;
        this.notificationService.info('Saved');
        this.loadState();
      },
      error => {
        this.saving = false;

        this.notificationService.notify(NotificationServiceHelper.deriveHttpErrorMessage('save', error));
        console.log(error);
      });
  }
}

export class ScheduledRelayState {
  power: boolean = false;
  time: string = '';
}
