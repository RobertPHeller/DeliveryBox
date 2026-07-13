//
//  ContentView.swift
//  DeliveryBox_iOS
//
//  Created by Robert Heller on 7/13/26.
//

import SwiftUI

struct ContentView: View {
    @State var ssid = ""
    @State var password = ""
    @State var masterCode = ""
    @State var oneTimeCode = ""
    var body: some View {
        VStack(alignment: .leading, spacing: 12) {
          Text("Delivery Box App").font(.title)
          Section(header: Text("Connection")) {
             VStack {
               HStack {
                 Button("Connect to Delivery Box") { }
                 Button("Disconnect from Delivery Box") { }
               }
               Text("*BLE State*")
             }
          }
          Section(header: Text("WiFi Configuration")) {
             VStack {
               TextField("SSID", text: $ssid)
               SecureField("Password", text: $password)
               Button("Configure WiFi") { }
             }
           }
           Section(header: Text("Set Master Code")) {
             VStack {
               TextField("Master Code:", text: $masterCode)
               Button("Set Master Code") { }
             }
           }
           Section(header: Text("Add One Time Code")) {
             VStack {
               TextField("One Time Code:", text: $oneTimeCode)
               Button("Add One Time Code") { }
             }
           }
           Section(header: Text("Reboot")) {
             Button("Reboot") { }
           }
        }   
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
