//
//  SharedCode.h
//  SignpostDemoServer
//
//  Created by Sebastian Eide on 08/05/2012.
//  Copyright (c) 2012 Kle.io. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum {
  /* Sent from client
   * When sending, the client initiates RTT tests.
   */
  PING, 
  
  /* Sent from server in response to PING
   * When sending, the server initiates RTT tests.
   * When received by the client, the client concludes RTT test.
   */
  PANG, 
  
  /* Sent from client in response to PANG
   * Upon receiving, the server concludes RTT test
   * Upon sending, the client starts timer for goodput test.
   */
  PONG, 
  
  /* Sent from server in response to PONG
   * DATASIZE bytes sent by server.
   * The client uses this to calculate the goodput.
   */
  DATAFROMSERVER,
  
  /* Sent from client in response to DATAFROMSERVER
   * The downstream bandwidth, as seen by the client.
   */
  DOWNSTREAM_BW,
  
  /* Sent from server in response to DOWNSTREAM_BW
   * Upon sending, the server starts timers for UPSTREAM goodput test.
   */
  PENG, 
  
  /* Sent by client in response to PENG
   * Upon receiving, the server concludes UPSTREAM goodput test.
   */
  DATAFROMCLIENT,
  
  /* Sent by server in response to DATAFROMCLIENT
   * UPSTREAM gootput as seen by the server.
   */
  UPSTREAM_BW,
  
////////////////////////////////////////////////////////////////////////////////////////////////////
  
  /* Sent by the client to server, to advertise what UDP port the client is listening to
   * for jitter measurements.
   */
  CLIENTJITTERPORT,
  
  /* The id of the client, as selfreported by the client.
   */
  CLIENTID,
  
  /* Sent by the server in response to a CLIENTJITTERPORT message. The message contains
   * the UDP port number the server will be listening to for jitter messages from the client.
   */  
  SERVERJITTERPORT,
  
  /* A message sent by the client or the server to the others jitter UDP port.
   * The message contains the parties current jitter estimate.
   */
  JITTERMESSAGE
} Messages;

// How many jitter messages to retain
#define JITTERMESSAGECOUNT 60 // slightly more than one second, given 50msg/s

#define INTERVAL_BETWEEN_JITTER_MESSAGES 20000000; // in nanoseconds. i.e. 50 times per second.

#define FORMAT(format, ...) [NSString stringWithFormat:(format), ##__VA_ARGS__]

#define READ_TIMEOUT 150.0
#define READ_TIMEOUT_EXTENSION 100.0

@interface SharedCode : NSObject 
{
  NSData *dataPayload;
  
  NSMutableDictionary *jitterMeasurements;
  NSMutableDictionary *jitterCache;
  
  dispatch_queue_t jitterCalcQueue;
  
  double latency;
  NSInteger numBytesForData;
}

@property (nonatomic, retain) NSString *hostname;

- (id)init;

// For changing amount of data to use for
// goodput measurement
- (void)setNumberOfBytesForDataMeasurements:(NSInteger)numKBytes;
- (NSInteger)numBytesToReadForData;


// Latency related functionality
- (NSDate *)startLatencyMeasurement;
- (double)concludeLatencyMeasurementForDate:(NSDate *)startTimeLatency;


// Bandwidth related functionality
- (NSDate *)startBandwidthMeasurement;
- (double) getBandwidthInMegabitsPerSecondForStartTime:(NSDate *)startTimeBandwidth;



// Data handling and conversion related functionality
- (NSData *) dataPayload;
+ (NSData *) intToData:(NSInteger)integerValue;
+ (NSInteger) dataToInt:(NSData *)data;
+ (NSData *) payloadForString:(NSString *)stringVal;
+ (NSString *) stringFromData:(NSData *)data;


// Jitter related functionality

// Returns the time in ms since the NSDate object in the packet 
// was generated.
+ (double) msFromTimestampData:(NSData *)data;
+ (NSString *) hostFromData:(NSData *)data;
+ (NSNumber *) hostJitterFromData:(NSData *)data;
- (NSData *) jitterPayloadContainingJitter:(NSNumber *)jitter;
- (void)addJitterMeasurement:(double)measurement forHost:(NSString*)host;
- (NSNumber *)currentJitterForHost:(NSString*)host;
- (void)performJitterMeasurements:(NSDictionary*)infoDict;
@end
