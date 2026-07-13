import XCTest
@testable import DeliveryBox

final class DeliveryBoxTests: XCTestCase {
    func testExample() {
        // This is an example of a functional test case.
        // Use XCTAssert and related functions to verify your tests produce the correct
        // results.
        XCTAssertEqual(DeliveryBox().text, "Hello, World!")
    }

    static var allTests = [
        ("testExample", testExample),
    ]
}
