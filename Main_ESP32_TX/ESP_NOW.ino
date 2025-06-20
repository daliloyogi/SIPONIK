void OnDataRecv(const esp_now_recv_info_t *recvInfo, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  char macStr[18];
  snprintf(macStr, sizeof(macStr),
           "%02X:%02X:%02X:%02X:%02X:%02X",
           recvInfo->src_addr[0], recvInfo->src_addr[1], recvInfo->src_addr[2],
           recvInfo->src_addr[3], recvInfo->src_addr[4], recvInfo->src_addr[5]);

  suhuRuang = incomingData.temperature;
}