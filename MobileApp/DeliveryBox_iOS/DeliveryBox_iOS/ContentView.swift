//
//  ContentView.swift
//  DeliveryBox_iOS
//
//  Created by Robert Heller on 7/13/26.
//

import SwiftUI

// ObservableObject that limits text fields to numbers (digits) only
class NumbersOnly: ObservableObject {
    @Published var value = "" {
        didSet {
            let filtered = value.filter { $0.isNumber }
                                    
            if value != filtered {
                value = filtered
            }
        }
    }
}


struct ContentView: View {
    // Connect to the BLE interface module
    var deliveryBoxManager = DeliveryBoxBLE()
    let characterLimit = 8
    // Variables to hold input values
    @State var ssid = ""
    @State var password = ""
    @ObservedObject var masterCode = NumbersOnly()
    @ObservedObject var oneTimeCode = NumbersOnly()
    var body: some View {
        VStack(alignment: .leading, spacing: 12) {
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
           }
           Section(header: Text("Set Master Code")) {
             VStack {
               TextField("Master Code:", text: $masterCode.value)
                          .keyboardType(.decimalPad)
                          .onChange(of: text) { newValue in
                              if newValue.count > characterLimit {
                                  text = String(newValue.prefix(characterLimit))
                              }
                           }
               Button("Set Master Code",
                        action: {
                            deliveryBoxManager.SendMasterCode(masterCode: masterCode)
                        }) 
             }
           }
           Section(header: Text("Add One Time Code")) {
             VStack {
               TextField("One Time Code:", text: $oneTimeCode.value)
                          .keyboardType(.decimalPad)
                          .onChange(of: text) { newValue in
                              if newValue.count > characterLimit {
                                  text = String(newValue.prefix(characterLimit))
                              }
                           }
               Button("Add One Time Code",
                        action: {
                            deliveryBoxManager.SendOneTimeCode(oneTimeCode: oneTimeCode)
                        }) 
             }
           }
           Section(header: Text("Reboot")) {
             Button("Reboot",
                    action: {
                     "   deliveryBoxManager.SendReboot()
                    }) 
           }
        }
        .padding()
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
