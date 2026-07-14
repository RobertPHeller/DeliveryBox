//
//  ContentView.swift
//  DeliveryBox_iOS
//
//  Created by Robert Heller on 7/13/26.
//

import SwiftUI

 
struct ContentView: View {
    // Connect to the BLE interface module
    var deliveryBoxManager = DeliveryBoxBLE()
    // Variables to hold input values
    @State var ssid = ""
    @State var password = ""
    //@State var masterCode = ""
    //@State var oneTimeCode = ""
    @ObservedObject var masterCode = NumbersOnly8()
    @ObservedObject var oneTimeCode = NumbersOnly8()
    var body: some View {
        VStack(alignment: .center, spacing: 12) {
          Text("Delivery Box App").font(.title)
          Section(header: Text("WiFi Configuration")) {
             VStack {
               TextField("SSID", text: $ssid)
               SecureField("Password", text: $password)
               Button("Configure WiFi",
                      action: {
                        deliveryBoxManager.SendWiFiConfigure(ssid: ssid,
                                                             password: password)
                      }) 
             }
             //.background(Color(red: 0, green: 1.0, blue: 0.0))
           }
           .buttonStyle(DelBoxButtonStyle())
           .frame(maxWidth: .infinity)
           Section(header: Text("Set Master Code")) {
             VStack {
               TextField("Master Code:", text: $masterCode.value)
                          .keyboardType(.decimalPad)
               Button("Set Master Code",
                        action: {
                            deliveryBoxManager.SendMasterCode(masterCode: masterCode.value)
                        }) 
               }
               //.background(Color(red: 0, green: 1.0, blue: 0.0))
           }
           .buttonStyle(DelBoxButtonStyle())
           .frame(maxWidth: .infinity)
           Section(header: Text("Add One Time Code")) {
             VStack {
               TextField("One Time Code:", text: $oneTimeCode.value)
                          .keyboardType(.decimalPad)
               Button("Add One Time Code",
                        action: {
                            deliveryBoxManager.SendOneTimeCode(oneTimeCode: oneTimeCode.value)
                        }) 
             }
             //.background(Color(red: 0, green: 1.0, blue: 0.0))
           }
           .buttonStyle(DelBoxButtonStyle())
           .frame(maxWidth: .infinity)
           Section(header: Text("Reboot")) {
             VStack {
                 Button("Reboot",
                    action: {
                       deliveryBoxManager.SendReboot()
                    }) 
              }
              //.background(Color(red: 0, green: 1.0, blue: 0.0))
           }
           .buttonStyle(DelBoxButtonStyle())
           .frame(maxWidth: .infinity)
           Link("Created by Robert Heller",
                    destination: URL(string: "https://www.thecountryrobot.com/")!)
                    .font(.footnote)
                    .foregroundColor(.blue)
                    .padding()
           Link("Full project here.", 
                    destination: URL(string: "https://github.com/RobertPHeller/DeliveryBox")!)
                    .font(.footnote)
                    .foregroundColor(.blue)
                    .padding()
        }
        .padding()
        .background(Color(red: 0.6784313725490196, 
                               green: 0.8470588235294118,
                               blue: 0.9019607843137255))
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
