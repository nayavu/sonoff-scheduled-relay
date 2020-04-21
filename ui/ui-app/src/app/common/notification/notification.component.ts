import { Component, OnDestroy, OnInit } from '@angular/core';
import { Message, NotificationService } from "./notification.service";
import { Subscription } from "rxjs";

@Component({
  selector: 'app-notification',
  template: `
      <div *ngIf="msg" class="alert" [ngClass]="msg.status == 'error' ? 'alert-danger' : ( msg.status === 'saved' ? 'alert-success' : 'alert-info') " role="alert">
          <button type="button" class="close" data-dismiss="alert" aria-label="Close" (click)="reset()">
              <span aria-hidden="true">&times;</span>
          </button>
          {{ msg.message }}
          <ng-container *ngIf="msg.details">: {{msg.details}}</ng-container>
      </div>`,
})
export class NotificationComponent implements OnInit, OnDestroy {
  msg: Message;
  subscription: Subscription;

  constructor(private notificationService: NotificationService) {
  }

  ngOnInit() {
    this.subscription = this.notificationService.messages.subscribe((msg) => this.msg = msg);
  }

  ngOnDestroy(): void {
    this.subscription.unsubscribe();
  }

  private reset() {
    this.msg = null;
  }
}
