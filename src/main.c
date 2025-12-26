#include <MCAL/GPIO/gpio.h>
#include <MCAL/RCC/rcc.h>
#include <HAL/HSerial/hserial.h>


volatile bool tx_done_flag = false;
volatile bool rx_done_flag = false;


void tx_do_smth(){
    tx_done_flag = true;
}

void rx_do_smth(){
    rx_done_flag = true;
}


// will use spi1 to test
HSerial_instance_t h = {
    .type = HSERIAL_TYPE_SPI,
    .spi_cfg = {
        .spi_num = SPI_NUM_1,
        .mode = SPI_MODE_MASTER,
        .speed = SPI_SPEED_DIV128
    },
    .rx_callback = rx_do_smth,
    .tx_callback = tx_do_smth
};



uint8_t to_send[3] = {67, 68, 69};
uint8_t to_receive[3] = {0};


int main(void)
{
    rcc_En_clk_preiph(RCC_GPIOA);

    HSerial_oop_inst_t inst = HSerial_oop_init(&h);

    inst.send(to_send, 3);

    inst.receive(to_receive, 3);

    while (1)
    {

    }
}
