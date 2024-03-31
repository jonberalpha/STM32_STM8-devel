int main(void)
{
    // Initialize I2C1, no pin remapping, no fast mode, APB1 clock is 8 MHz
    i2c_init(I2C1, false, false, 8000000);

    // Send and receive buffer
    uint8_t send_buffer[4]={10,20,30,40};
    uint8_t receive_buffer[9];

    // Communicate with slave address 8: send 4 bytes, then receive 9 bytes
    int received=i2c_communicate(I2C1, 8, send_buffer, 4, receive_buffer, 9);
}