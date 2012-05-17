//
//  ViewController.h
//  SignpostDemoiPhone
//
//  Created by Sebastian Eide on 09/05/2012.
//  Copyright (c) 2012 Kle.io. All rights reserved.
//

#import <UIKit/UIKit.h>

@class GCDAsyncSocket;
@class GCDAsyncUdpSocket;
@class SharedCode;
@class Meter;
@class LatencyGoodputView;

@interface ViewController : UIViewController <UITextFieldDelegate>
{
  // Latency and goodput
  dispatch_queue_t socketQueue;
	GCDAsyncSocket *socket;
  
  // Jitter
  dispatch_queue_t jitterSocketQueue;
	GCDAsyncUdpSocket *jitterSocket;
  
  // General bookkeeping
	BOOL isConnected;
  
  SharedCode *commonFunc;
  
  // For calculating jitter
  NSUInteger serverJitterPort;
  NSMutableArray *interarrivalTimesOfJitterMessages;
  NSDate *lastReceivedMessage;  
  
  double serverJitter;
  double averageJitter;
  NSString *jitterHost;
  
  Meter *meter;
  
  double clientLatency;
  double serverLatency;
  
  double downstreamBandwidth;
  double upstreamBandwidth;
  NSDate *startTimeLatency, *startTimeBandwidth;
}

@property (assign) IBOutlet UIButton *connectButton;
@property (assign) IBOutlet UITextField *hostField;
@property (assign) IBOutlet UITextField *portField;
@property (assign) IBOutlet UIView *connectView;
@property (assign) IBOutlet UIView *connectViewControls;
@property (assign) IBOutlet UIView *connectViewFadeout;
@property (assign) IBOutlet LatencyGoodputView *latencyGoodputView;

- (IBAction)connectToHostButtonClicked:(id)sender;
@end
