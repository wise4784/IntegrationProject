/*
** Interrupt Handler for Core 0 Receive interrupt
*/

/*
** lwIP Compile Time Options for HDK.
*/
#include "lwiplib.h"
#include "HL_sci.h"
//#include "lwip\inet.h"
#include "inet.h"
#include "locator.h"

#define sciREGx  sciREG1

uint8_t    txtCRLF[]      = {'\r', '\n'};
uint8_t    txtErrorInit[]    = {"-------- ERROR INITIALIZING HARDWARE --------"};
uint8_t    txtIPAddrTxt[]    = {"Device IP Address: "};
uint8_t     * txtIPAddrItoA;

volatile int countEMACCore0RxIsr = 0;
#pragma INTERRUPT(EMACCore0RxIsr, IRQ)
void EMACCore0RxIsr(void)
{
    countEMACCore0RxIsr++;
    lwIPRxIntHandler(0);
}

/*
** Interrupt Handler for Core 0 Transmit interrupt
*/
volatile int countEMACCore0TxIsr = 0;
#pragma INTERRUPT(EMACCore0TxIsr, IRQ)
void EMACCore0TxIsr(void)
{
  countEMACCore0TxIsr++;
    lwIPTxIntHandler(0);
}

void IntMasterIRQEnable(void)
{
  _enable_IRQ();
  return;
}

void IntMasterIRQDisable(void)
{
  _disable_IRQ();
  return;
}

unsigned int IntMasterStatusGet(void)
{
    return (0xC0 & _get_CPSR());
}

void sciDisplayText(sciBASE_t *sci, uint8_t *text,uint32_t length)
{
    while(length--)
    {
        while ((sci->FLR & 0x4) == 4); /* wait until busy */
        sciSendByte(sci,*text++);      /* send out text   */
    };
}

void EMAC_LwIP_Main (uint8_t * macAddress)
{
    unsigned int   ipAddr;
    struct in_addr   devIPAddress;

  sciInit();

  /* Enable the interrupt generation in CPSR register */
  IntMasterIRQEnable();
  _enable_FIQ();


  /* Initialze the lwIP library, using DHCP.*/
  //ipAddr = lwIPInit(0, macAddress, 0, 0, 0, IPADDR_USE_DHCP);

  /* Initialize the lwIP library, using Static.*/
#if 1
    uint8 ip_addr[4] = { 192, 168, 3, 77 };
    uint8 netmask[4] = { 255, 255, 255, 0 };
    uint8 gateway[4] = { 192, 168, 3, 1 };
    ipAddr = lwIPInit(0, macAddress,
            *((uint32_t *)ip_addr),
            *((uint32_t *)netmask),
            *((uint32_t *)gateway),
            IPADDR_USE_STATIC);
#endif

  if (0 == ipAddr) {
    sciDisplayText(sciREGx, txtCRLF, sizeof(txtCRLF));
    sciDisplayText(sciREGx, txtCRLF, sizeof(txtCRLF));
        sciDisplayText(sciREGx, txtErrorInit, sizeof(txtErrorInit));
        sciDisplayText(sciREGx, txtCRLF, sizeof(txtCRLF));
        sciDisplayText(sciREGx, txtCRLF, sizeof(txtCRLF));
  } else {
    /* Convert IP Address to string */
    devIPAddress.s_addr = ipAddr;
    txtIPAddrItoA = (uint8_t *)inet_ntoa(devIPAddress);
    LocatorConfig(macAddress, "LaunchPad enet_lwip");
    sciDisplayText(sciREGx, txtCRLF, sizeof(txtCRLF));
    sciDisplayText(sciREGx, txtCRLF, sizeof(txtCRLF));
    sciDisplayText(sciREGx, txtIPAddrTxt, sizeof(txtIPAddrTxt));
        sciDisplayText(sciREGx, txtIPAddrItoA, 16);
    sciDisplayText(sciREGx, txtCRLF, sizeof(txtCRLF));

       /* Loop forever.  All the work is done in interrupt handlers. */
      while(1)
      {
      }
  }
}
