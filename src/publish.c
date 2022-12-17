#include <stdlib.h>
#include <stdio.h>
#include <C:\Program Files (x86)\mosquitto\devel\mosquitto.h>

int main(){

    //variables
	int connect;
	struct mosquitto * mqtt;


	mosquitto_lib_init();
	mqtt = mosquitto_new("project", true, NULL);
	connect = mosquitto_connect(mqtt, "localhost", 1883, 60);


    //checks to see if it is connected or not
	if(connect != 0){

		printf("Client could not connect to broker! Error Code: %d\n", connect);
		mosquitto_destroy(mqtt);
		return -1;
    
	}

    //connected to the broker
	printf("We are now connected to the broker!\n");

	mosquitto_publish(mqtt, NULL, "project", 6, "Hello", 0, false);
	mosquitto_disconnect(mqtt);
	mosquitto_destroy(mqtt);

	mosquitto_lib_cleanup();
	return 0;
}