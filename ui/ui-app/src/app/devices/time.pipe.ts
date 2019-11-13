import { Pipe, PipeTransform } from "@angular/core";

@Pipe({name: 'time'})
export class TimePipe implements PipeTransform {

    static numberToTime(value: number): string {
        const formatter = Intl.NumberFormat('en', {minimumIntegerDigits: 2, maximumFractionDigits: 0});
        const sign =  value < 0 ? '-' : '';
        const mins = Math.abs(value) % 60;
        const hours = (Math.abs(value) - mins) / 60;

        return sign + formatter.format(hours) + ':' +  formatter.format(mins);
    }

    static timeToNumber(value: string): number {
        if (!value.match(/^-?\d{2}:\d{2}$/)) {
            return NaN;
        }
        const sign = value.startsWith('-') ? -1 : 1;
        const t = value.split(':');
        return sign * (parseInt(t[0]) * 60 + parseInt(t[1]));
    }

    transform(value: number): string {
        return TimePipe.numberToTime(value);
    }
}
