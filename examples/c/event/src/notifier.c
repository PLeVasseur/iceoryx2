// Copyright (c) 2024 Contributors to the Eclipse Foundation
//
// See the NOTICE file(s) distributed with this work for additional
// information regarding copyright ownership.
//
// This program and the accompanying materials are made available under the
// terms of the Apache Software License 2.0 which is available at
// https://www.apache.org/licenses/LICENSE-2.0, or the MIT license
// which is available at https://opensource.org/licenses/MIT.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT

#include "iox2/iceoryx2.h"

#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN64
#include <windows.h>
#else
#include <unistd.h>
#endif

int main(void) {
    // create new node
    iox2_node_builder_h node_builder_handle = iox2_node_builder_new(NULL);
    iox2_node_h node_handle = NULL;
    if (iox2_node_builder_create(node_builder_handle, NULL, iox2_service_type_e_IPC, &node_handle) != IOX2_OK) {
        printf("Could not create node!\n");
        goto end;
    }

    // create service name
    const char* service_name_value = "MyEventName";
    iox2_service_name_h service_name = NULL;
    if (iox2_service_name_new(NULL, service_name_value, strlen(service_name_value), &service_name) != IOX2_OK) {
        printf("Unable to create service name!\n");
        goto drop_node;
    }

    // create service
    iox2_service_name_ptr service_name_ptr = iox2_cast_service_name_ptr(service_name);
    iox2_node_ref_h node_ref_handle = iox2_cast_node_ref_h(node_handle);
    iox2_service_builder_h service_builder = iox2_node_service_builder(node_ref_handle, NULL, service_name_ptr);
    iox2_service_builder_event_h service_builder_event = iox2_service_builder_event(service_builder);
    iox2_service_builder_event_ref_h service_builder_event_ref =
        iox2_cast_service_builder_event_ref_h(service_builder_event);
    iox2_port_factory_event_h service = NULL;
    if (iox2_service_builder_event_open_or_create(service_builder_event, NULL, &service) != IOX2_OK) {
        printf("Unable to create service!\n");
        goto drop_node;
    }

    // create notifier
    iox2_port_factory_event_ref_h ref_service = iox2_cast_port_factory_event_ref_h(service);
    iox2_port_factory_notifier_builder_h notifier_builder = iox2_port_factory_event_notifier_builder(ref_service, NULL);
    iox2_notifier_h notifier = NULL;
    if (iox2_port_factory_notifier_builder_create(notifier_builder, NULL, &notifier) != IOX2_OK) {
        printf("Unable to create notifier!\n");
        goto drop_service;
    }
    iox2_notifier_ref_h notifier_ref = iox2_cast_notifier_ref_h(notifier);
    iox2_event_id_t event_id;

    uint64_t counter = 0;
    while (iox2_node_wait(node_ref_handle, 0, 0) == iox2_node_event_e_TICK) {
        counter += 1;
        iox2_event_id_t event_id = { .value = counter % 12 };
        if (iox2_notifier_notify_with_custom_event_id(notifier_ref, &event_id, NULL) != IOX2_OK) {
            printf("Failed to notify listener!\n");
            goto drop_notifier;
        }

        printf("Trigger event with id %lu ...\n", event_id.value);

        sleep(1);
    }

drop_notifier:
    iox2_notifier_drop(notifier);

drop_service:
    iox2_port_factory_event_drop(service);

drop_node:
    iox2_node_drop(node_handle);

end:
    return 0;
}
