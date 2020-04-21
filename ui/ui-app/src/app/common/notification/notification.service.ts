import { Injectable } from "@angular/core";
import { ReplaySubject } from "rxjs";

@Injectable({
  providedIn: 'root'
})
export class NotificationService {
  public messages = new ReplaySubject<Message>();

  public info(message: string, details: string = null) {
    this.notify({ status: 'info', message, details });
  }

  public warn(message: string, details: string = null) {
    this.notify({ status : 'warn', message, details });
  }

  public error(message: string, details: string = null) {
    this.notify({ status: 'error', message, details });
  }

  public notify(message: Message) {
    this.messages.next(message);
  }

  public reset() {
    this.messages.next(null);
  }
}

export class NotificationServiceHelper {

  public static deriveHttpErrorMessageForLoadAction(action: string, error: any): Message {
    if (error.status == 404) {
      return { status: 'info', message: 'Could not ' + action, details: 'no data found' };
    }
    return NotificationServiceHelper.deriveHttpErrorMessage(action, error);
  }

  public static deriveHttpErrorMessage(action: string, error: any): Message {
    let message, details;
    if (error.status == 404) {
      message = 'Endpoint for ' + action + ' not found';
    } else {
      message = 'Failed to ' + action;
      if (error.status == 500) {
        details = 'data on device is corrupted';
      } else if (!error.status) {
        details = 'device is unreachable';
      }
    }
    return { status: 'error', message, details };
  }
}

export interface Message {
  status: string,
  message: string,
  details?: string
}