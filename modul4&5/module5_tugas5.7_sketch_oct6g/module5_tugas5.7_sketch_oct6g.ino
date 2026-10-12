#include <painlessMesh.h>
#include <ArduinoJson.h>

#define MESH_PREFIX     "LabIoTMesh"
#define MESH_PASSWORD   "iotmeshpassword"
#define MESH_PORT       5555

#define LDR_PIN         A0      // Pin ADC A0 NodeMCU

Scheduler userScheduler;
painlessMesh mesh;

void sendMessage();
Task taskSendMessage(TASK_SECOND * 3, TASK_FOREVER, &sendMessage);

void sendMessage() {
  int nilaiLDR = analogRead(LDR_PIN);

  // Kemas data ke format JSON sesuai spesifikasi tugas
  StaticJsonDocument<200> doc;
  doc["tipe"] = "cahaya_node";
  doc["adc"] = nilaiLDR;

  String msg;
  serializeJson(doc, msg);
  mesh.sendBroadcast(msg);

  Serial.print("[KIRIM LDR] ");
  Serial.println(msg);
}

void receivedCallback(uint32_t from, String &msg) {
  // Opsional
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> Koneksi Baru Terdeteksi! Node ID: %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.println("--> Topologi mesh diperbarui");
}

void setup() {
  Serial.begin(115200);

  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();

  Serial.println("Node 2 (Sensor LDR) Siap!");
}

void loop() {
  mesh.update();
}