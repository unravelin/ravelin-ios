# Getting Started

The Ravelin Mobile SDKs are critical to our ability to provide informed decisions to our clients. 

Like the JavaScript library, the SDK enables:

* The generation of a unique and persistent device ID
* The collection of additional device details
* Session tracking 
* If relevant to your integration, the encryption of card details 

We have two frameworks within the SDK:

* RavelinCore.framework - Which is used for deviceId, fingerprinting and tracking activity
* RavelinEncrypt.framework - for card details encryption

You can chose what functionality of the SDK you would like to use. However, at a mimimum we advise that you use the SDK to generate a reliable device ID and to send the additional device details for your app traffic. Device IDs are critical throughout our fraud prevention tool, especially for use in our graph database.  

## Getting Started

Before you can integrate with the Ravelin mobile SDK for iOS, you will need:

* to locate RavelinCore and/or RavelinEncrypt on CocoaPods (if you want to install via Cocoapods). You only need the RavelinCore pod if you do not need to use the encryption functionality.
* valid API keys which are available in the Ravelin Dashboard in the account menu under the Developer option

If you have any questions on getting started, please ask us in your Ravelin support channel on Slack.

## Further information

* [Further information on using RavelinCore](https://github.com/unravelin/ravelin-ios/blob/0.3.0-docs/RavelinCore.md)
* [Further information on using RavelinEncrypt](https://github.com/unravelin/ravelin-ios/blob/0.3.0-docs/RavelinEncrypt.md)
