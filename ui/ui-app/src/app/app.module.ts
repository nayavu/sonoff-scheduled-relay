import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { SettingsComponent } from './settings/settings.component';
import { RelayComponent } from './relay/relay.component';
import { FormsModule, ReactiveFormsModule } from "@angular/forms";
import { HttpClientModule } from "@angular/common/http";
import { TimeInputComponent } from './common/time-input/time-input.component';
import { NotificationComponent } from './common/notification/notification.component';
import { NotificationService } from "./common/notification/notification.service";

@NgModule({
  declarations: [
    AppComponent,
    SettingsComponent,
    RelayComponent,
    TimeInputComponent,
    NotificationComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    FormsModule,
    ReactiveFormsModule
  ],
  providers: [
    NotificationService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
