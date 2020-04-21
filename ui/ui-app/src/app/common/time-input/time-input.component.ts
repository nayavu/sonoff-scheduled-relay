import { Component, forwardRef, Input, OnInit } from '@angular/core';
import {
  ControlValueAccessor,
  FormControl,
  NG_VALIDATORS,
  NG_VALUE_ACCESSOR,
  ValidationErrors,
  Validator
} from "@angular/forms";

/**
 * Allows to input time in hh:mm format transforming the input value into minutes, i.e. 01:05 -> 65 minutes
 */
@Component({
  selector: 'app-time-input',
  template: `<input 
                type="text" 
                placeholder="{{placeholder}}"
                title="Time in format {{placeholder}}"
                class="form-control"
                [value]="textValue"
                [disabled]="disabled"
                (keydown)="onKeyDown($event)"
                (input)="onSetTextValue($event)"
                (blur)="this.onTouch()"
                />`,
  providers: [{
    provide: NG_VALUE_ACCESSOR,
    useExisting: forwardRef(() => TimeInputComponent),
    multi: true
  }, {
    provide: NG_VALIDATORS,
    useExisting: forwardRef(() => TimeInputComponent),
    multi: true
  }]
})
export class TimeInputComponent implements ControlValueAccessor, Validator, OnInit {
  @Input('value')
  private value: number; // value in minutes

  @Input('allowNegative')
  private allowNegative = false;

  textValue: string = '';
  disabled = false;

  onChange: any = () => {};
  onTouch: any = () => {};

  placeholder = 'hh:mm';

  private pattern;
  private timeFormatter = Intl.NumberFormat('en', { minimumIntegerDigits: 2, maximumFractionDigits: 0 });

  ngOnInit() {
    this.placeholder = this.allowNegative ? 'hh:mm or -hh:mm' : 'hh:mm';
    this.pattern = new RegExp('^' + (this.allowNegative ? '-?' : '') + '\\d{1,2}:\\d{2}$');
  }

  onSetTextValue(event) {
    this.textValue = (event.target as HTMLInputElement).value;
    this.value = this.textToMinutes(this.textValue);
    this.onChange(this.value);
  }

  onKeyDown(event: KeyboardEvent) {
    const key = event.key;
    return key == 'Tab' || key == 'ArrowLeft' || key == 'ArrowRight' || key == 'Home' || key == 'End' || key == 'Backspace' || key == 'Delete'
      || (key >= '0' && key <= '9') || key == ':' || (key == '-' && this.allowNegative);
  }

  registerOnChange(fn: any): void {
    this.onChange = fn;
  }

  registerOnTouched(fn: any): void {
    this.onTouch = fn;
  }

  writeValue(obj: any): void {
    this.value = obj;
    const val = this.minutesToText(this.value);
    if (val || !this.textValue) {
      this.textValue = val;
    }
  }

  setDisabledState(isDisabled: boolean) {
    this.disabled = isDisabled;
  }

  validate(c: FormControl): ValidationErrors | null {
    if (isNaN(this.value) || Math.abs(this.value) > 23 * 60 + 59 || (!this.allowNegative && this.value < 0)) {
      return { valid: false };
    } else {
      return null;
    }
  }

  private minutesToText(value: number): string {
    if (isNaN(value)) {
      return null;
    }

    const sign =  value < 0 ? '-' : '';
    const mins = Math.abs(value) % 60;
    const hours = (Math.abs(value) - mins) / 60;

    return sign + this.timeFormatter.format(hours) + ':' + this.timeFormatter.format(mins);
  }

  private textToMinutes(value: string): number {
    if (!value || !value.match(this.pattern)) {
      return NaN;
    }
    const sign = value.startsWith('-') ? -1 : 1;
    const t = value.split(':');
    return parseInt(t[0]) * 60 + sign * parseInt(t[1]);
  }
}
