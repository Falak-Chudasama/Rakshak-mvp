#pragma once
void init_lora_node();
int lora_receive_alert();
void lora_send_ack(int id);
bool lora_wait_ack_ack(int id, unsigned long timeout_ms = 3000);