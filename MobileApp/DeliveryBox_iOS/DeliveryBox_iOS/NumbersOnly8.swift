//
//  NumbersOnly8.swift
//  DeliveryBox_iOS
//
//  Created by Robert Heller on 7/13/26.
//

import SwiftUI

// ObservableObject that limits text fields to 8 numbers (digits) only
class NumbersOnly8: ObservableObject {
    let characterLimit = 8
    @Published var value = "" {
        didSet {
            let filtered = value.filter { $0.isNumber }
                                    
            if value != filtered {
                value = filtered
            }
            if value.count > characterLimit {
                value = String(value.prefix(characterLimit))
            }
        }
    }
}
