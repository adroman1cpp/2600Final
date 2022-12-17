#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>

int main(){
    //variables
	int connect, id=12;

	mosquitto_lib_init();

	struct mosquitto *mqtt;

	mqtt = mosquitto_new("subscribe-test", true, &id);
	mosquitto_connect_callback_set(mqtt, connect);
	mosquitto_message_callback_set(mqtt, message);
	
	connect = mosquitto_connect(mqtt, "localhost", 1883, 10);

    //checks if connected
	if(connect) {
		printf("Could not connect to Broker. Code: %d\n", connect);
		return -1;
	}

	//gets input
	mosquitto_loop_start(mqtt);
	printf("Press Enter to quit\n");
	getchar();
	mosquitto_loop_stop(mqtt, true);
	//disconnects from mqtt
	mosquitto_disconnect(mqtt);
	mosquitto_destroy(mqtt);
	mosquitto_lib_cleanup();
	
	return 0;
}

//function that connects
void connect(struct mosquitto *mqtt, void *obj, int connect) {
	printf("ID: %d\n", * (int *) obj);
	if(connect){
		printf("Error with result code: %d\n", connect);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, "project", 0);
}

//function that does the message
void message(struct mosquitto *mqtt, void *obj, const struct mosquitto_message *msg) {
	printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
}