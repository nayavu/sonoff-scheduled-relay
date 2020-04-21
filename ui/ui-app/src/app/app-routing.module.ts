import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { SettingsComponent } from "./settings/settings.component";
import { RelayComponent } from "./relay/relay.component";

const routes: Routes = [
  { path: '', redirectTo: '/relay', pathMatch: 'full' },
  { path: 'relay', component: RelayComponent },
  { path: 'settings', component: SettingsComponent }
];

@NgModule({
  imports: [RouterModule.forRoot(routes, {useHash: true})],
  exports: [RouterModule]
})
export class AppRoutingModule { }
