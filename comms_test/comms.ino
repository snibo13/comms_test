#include <comms.h>
#include <Wire.h>

static comms_state_t comms_state;

void initComms(BLDCMotor* motor, bool debug)
{
  comms_state.motor = motor;
  comms_state.initialised = true;
  initialisei2c();
}

void initialisei2c()
{
  Serial.println("Initialising I2C");
  Wire.begin(ADDR);
  Serial.print("Addr: ");
  Serial.println(ADDR);
  Wire.setSCL(SCL);
  Wire.setSDA(SDA);
  Wire.onReceive(recv_message);
  Wire.onRequest(req_message);
}

bool valid_packet(byte *packet)
{
  // Check if the command is valid
  if (packet[0] > COMMAND_MAX_INDX)
    return false;

  // Verify the checksum
  // For num using a simple XOR checksum could use CRC if reliability is an issue
  if (compute_checksum(packet[0], packet[1]) != packet[2])
    return false;

  // Default case return true unless specific flaw with the packet
  return true;
}

void req_message()
{
    // byte startByte = Wire.read();
    byte command = Wire.read();
    Serial.print(command);
    delay(500);
    Serial.print(":");
    delay(500);
    byte param = Wire.read();
    Serial.print(param);
    delay(500);
    Serial.print(":");
    delay(500);
    byte checksum = Wire.read();
    Serial.println(checksum);
    delay(500);
    byte packet[PACKET_SIZE] = {command, param, checksum};
    if (!valid_packet(&packet[0]))
    {
      Serial.println("Invalid packet");
      delay(500);
      return;
    }
    if (!comms_state.initialised)
    {
      Serial.println("Comms not initialised");
      delay(500);
      return;
    }

    switch (command)
    {
    case COMM_STOP:
      comm_stop_callback();
      break;
    case COMM_ENABLE:
      comm_enable_callback();
      break;
    case COMM_VEL:
      comm_vel_callback();
      break;
    case COMM_POS:
      comm_pos_callback();
      break;
    case COMM_TELEMETRY:
      comm_telemetry_callback();
      break;
    case COMM_TORQUE:
      comm_torque_callback(param);
      break;
    default:
      break;
    }
  
}

void recv_message(int numBytes)
{
  Serial.println("Message received!");
  Serial.println(numBytes);
  delay(500);
  if (numBytes == PACKET_SIZE)
  { 
    // byte startByte = Wire.read();
    byte command = Wire.read();
    Serial.print(command);
    delay(500);
    Serial.print(":");
    delay(500);
    byte param = Wire.read();
    Serial.print(param);
    delay(500);
    Serial.print(":");
    delay(500);
    byte checksum = Wire.read();
    Serial.println(checksum);
    delay(500);
    byte packet[PACKET_SIZE] = {command, param, checksum};
    if (!valid_packet(&packet[0]))
    {
      Serial.println("Invalid packet");
      delay(500);
      return;
    }
    if (!comms_state.initialised)
    {
      Serial.println("Comms not initialised");
      delay(500);
      return;
    }
    // TODO: Setup better error response than simply nothing

    switch (command)
    {
    case COMM_STOP:
      comm_stop_callback();
      break;
    case COMM_ENABLE:
      comm_enable_callback();
      break;
    case COMM_VEL:
      comm_vel_callback();
      break;
    case COMM_POS:
      comm_pos_callback();
      break;
    case COMM_TELEMETRY:
      comm_telemetry_callback();
      break;
    case COMM_TORQUE:
      comm_torque_callback(param);
      break;
    default:
      break;
    }
  }
}


void send_message(byte *packet)
{
  Serial.println("Sending message:");
  Serial.println(packet[0]);
  Serial.println(packet[1]);
  Serial.println(packet[2]);
  Wire.beginTransmission(0);
  // for (int i = 0; i < PACKET_SIZE; ++i)
  // {
    Wire.write(packet, 3);
  // }
  Wire.endTransmission();
  delay(100); // Unnecessary?
}

/* Callbacks */
void comm_stop_callback()
{
  Serial.println("COMM_STOP");
  delay(500);
  // Sets the target torque to zero and disables further listening
  comms_state.motor->target = 0;
  // comms_state.motor->disable();
}

void comm_enable_callback()
{
  Serial.println("COMM_EN");
  delay(500);
  // Enables communication to listen to commands
  Serial.println("Setting motor to enabled");
  // comms_state.motor->enable();
  Serial.println("Motor updated");
}

void comm_vel_callback()
{
  Serial.println("COMM_VEL");
  delay(500);
  byte comm = COMM_VEL;
  byte param = comms_state.motor->shaftVelocity();
  byte checksum = compute_checksum(comm, param); // To allow for easy checksum replacement if necessary

  byte packet[3] = {comm, param, checksum};
  send_message(packet);
}

void comm_pos_callback()
{
  Serial.println("COMM_POS");
  delay(500);
  byte comm = COMM_POS;
  byte param = comms_state.motor->shaftAngle();
  byte checksum = compute_checksum(comm, param); // To allow for easy checksum replacement if necessary

  byte packet[3] = {comm, param, checksum};
  send_message(packet);
}

void comm_torque_callback(byte param)
{
  Serial.println("COMM_TORQUE");
  delay(500);
  comms_state.motor->target = param;
}

void comm_telemetry_callback()
{
  Serial.println("COMM_TELEM");
  delay(500);
  comm_pos_callback();
  delay(100);
  comm_vel_callback();
}



/* Helpers */
byte compute_checksum(byte comm, byte param)
{
  return comm ^ param; // Simple XOR checksum
}