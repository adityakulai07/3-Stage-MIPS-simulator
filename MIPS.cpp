#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.


class RF
{
    public:
        bitset<32> ReadData1, ReadData2;
     	RF()
    	{
          Registers.resize(32);
          Registers[0] = bitset<32> (0);
        }

        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {
            // implement the funciton by you.
            ReadData1 = Registers[RdReg1.to_ulong()];
            ReadData2 = Registers[RdReg2.to_ulong()];

            int WriteEnable = WrtEnable.to_ulong();

            if(WriteEnable==1){
              Registers[WrtReg.to_ulong()] = WrtData;
            }

         }

	void OutputRF()
             {
               ofstream rfout;
                  rfout.open("RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {
                        rfout << Registers[j]<<endl;
                      }

                  }
                  else cout<<"Unable to open file";
                  rfout.close();

               }
	private:
            vector<bitset<32> >Registers;

};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {
                 // implement the ALU operations by you.
                 int opr1,opr2,aluop,aluresult;
                 opr1 = oprand1.to_ulong();
                 opr2 = oprand2.to_ulong();
                 aluop = ALUOP.to_ulong();
                 aluresult= ALUresult.to_ulong();

                 //Now using the if-else statement for ALU operations

                 if(aluop == 1){
                   aluresult = opr1+opr2;
                 }
                 else if(aluop==3){
                   aluresult = opr1-opr2;
                 }
                 else if(aluop == 4){
                   aluresult = opr1&opr2;
                 }
                 else if(aluop == 5){
                   aluresult = opr1 | opr2;
                 }
                 else if(aluop == 7){
                   aluresult = !(opr1 | opr2);
                 }

                 std::bitset<32> ALUresult(aluresult);
                  return ALUresult;




               }
};

class INSMem
{
      public:
          bitset<32> Instruction;
          INSMem()
          {       IMem.resize(MemSize);
                  ifstream imem;
                  string line;
                  int i=0;
                  imem.open("imem.txt");
                  if (imem.is_open())
                  {
                  while (getline(imem,line))
                     {
                        IMem[i] = bitset<8>(line);
                        i++;
                     }

                  }
                  else cout<<"Unable to open file";
                  imem.close();

                  }

          bitset<32> ReadMemory (bitset<32> ReadAddress)
              {

               // implement by you. (Read the byte at the ReadAddress and the following three byte).


               Instruction = bitset<32>((IMem[ReadAddress.to_ulong()].to_ulong())<<24 | (IMem[ReadAddress.to_ulong()+1].to_ulong())<<16 | (IMem[ReadAddress.to_ulong()+2].to_ulong())<<8 | (IMem[ReadAddress.to_ulong()+3].to_ulong()));
               return Instruction;
              }

      private:
           vector<bitset<8> > IMem;

};

class DataMem
{
      public:
          bitset<32> readdata;
          DataMem()
          {
             DMem.resize(MemSize);
             ifstream dmem;
                  string line;
                  int i=0;
                  dmem.open("dmem.txt");
                  if (dmem.is_open())
                  {
                  while (getline(dmem,line))
                       {
                        DMem[i] = bitset<8>(line);
                        i++;
                       }
                  }
                  else cout<<"Unable to open file";
                  dmem.close();

          }
          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
          {

               // implement by you.

               if(readmem == 1){
                 readdata = bitset<32> ((DMem[Address.to_ulong()].to_ulong())<<24 | (DMem[Address.to_ulong()+1].to_ulong())<<16 | (DMem[Address.to_ulong()+2].to_ulong())<<8 | (DMem[Address.to_ulong()+3].to_ulong()));
               }
               else if(writemem == 1){
                 (DMem[Address.to_ulong()]) = bitset<8> (WriteData.to_string().substr(0,8));
                 (DMem[Address.to_ulong()+1]) = bitset<8> (WriteData.to_string().substr(8,8));
                 (DMem[Address.to_ulong()+2]) = bitset<8> (WriteData.to_string().substr(16,8));
                 (DMem[Address.to_ulong()+3]) = bitset<8> (WriteData.to_string().substr(24,8));
               }
               return readdata;
          }

          void OutputDataMem()
          {
               ofstream dmemout;
                  dmemout.open("dmemresult.txt");
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                       {
                        dmemout << DMem[j]<<endl;
                       }

                  }
                  else cout<<"Unable to open file";
                  dmemout.close();

               }

      private:
           vector<bitset<8> > DMem;

};

void dumpResults(bitset<32> pc, bitset<5> WrRFAdd, bitset<32> WrRFData, bitset<1> RFWrEn, bitset<32> WrMemAdd, bitset<32> WrMemData, bitset<1> WrMemEn)
{
                  ofstream fileout;

                  fileout.open("Results.txt",std::ios_base::app);
                  if (fileout.is_open())
                  {

                  fileout <<pc<<' '<<WrRFAdd<<' '<<WrRFData<<' '<<RFWrEn<<' '<<WrMemAdd<<' '<<WrMemData<<' '<<WrMemEn << endl;

                  }
                  else cout<<"Unable to open file";
                  fileout.close();

}


int main()
{
    bitset<32> pc=0;
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;


    bitset<6> opcode;
    bitset<5> RegRd;
    bitset<5> RegRt;
    bitset<5> RegRs;
    bitset<32> ALUoutput;
    bitset<32> opr1;
    bitset<32> opr2;
    bitset<32> Instruction;
    bitset<32> written_output_rf;
    bitset<5> written_add_rf;
    bitset<32> data_from_mem;
    bitset<8> written_output;
    bitset<1> write_mem_en;
    bitset<1> write_rf_en;
    bitset<32> written_add_mem;
    bitset<32> written_data_mem;


    while (1) //each loop body corresponds to one clock cycle.
	{
    //Stage 3(Mem and Write back)
    cout<<"Stage 3"<<"/n";
    //R INSTRUCTION
    if(opcode !=2 || opcode !=4)     //if opcode is not a branch or a jump Instruction, then execute the code snippet below
    {

    myRF.ReadWrite(0,0,RegRd,ALUoutput,1);
    myRF.ReadWrite(RegRd,0,0,0,0);
    written_add_rf = RegRd;
    written_output_rf = myRF.ReadData1;
    write_rf_en = 1;
    write_mem_en = 0 ;
    }

    //LOAD INSTRUCTION

    if(opcode == 35)
    {
    data_from_mem = myDataMem.MemoryAccess(ALUoutput,0,1,0); //DATA FROM DATA MEMORY
    myRF.ReadWrite(0,0,RegRt,data_from_mem,1);
    myRF.ReadWrite(0,RegRt,0,0,0);

    written_output_rf = myRF.ReadData2;    //data loaded to register after load inst
    written_add_rf = RegRd;
    write_rf_en = 1;
    write_mem_en = 0 ;
    }
    //STORE

    if(opcode == 43)
    {
    myRF.ReadWrite(0,RegRt,0,0,0);
    written_output_rf = myRF.ReadData2;
    written_add_rf = RegRt;
    myDataMem.MemoryAccess(ALUoutput,written_output_rf,0,1);
    myDataMem.MemoryAccess(ALUoutput,0,1,0 );

    written_add_mem = ALUoutput;
    written_data_mem = myDataMem.readdata;
    write_rf_en = 0;
    write_mem_en = 1 ;
    }
    //ADDIU INSTRUCTION
    if(opcode == 9)
    {
      myRF.ReadWrite(0,0,RegRt,ALUoutput,1);
      myRF.ReadWrite(RegRt,0,0,0,0);
      written_add_rf = RegRt;
      written_output_rf = myRF.ReadData1;
      write_rf_en = 1;
      write_mem_en = 0 ;

    }

    //Stage 2(ID/RF and EX)
    cout<<"Stage 2"<<"/n";

    if(Instruction==4294967295){              //HALT CASE: 4294967295 = "11111111111111111111111111111111"
      break;
    }
    else{
    bitset<6> opcode(Instruction.to_string().substr(0,6));
    //R type
    if(opcode==0){
      cout<<"R-TYPE"<<"/n";
      bitset<5> RegRs(Instruction.to_string().substr(6,5));
      bitset<5> RegRt(Instruction.to_string().substr(11,5));
      bitset<5> RegRd(Instruction.to_string().substr(16,5));
      bitset<3> Func(Instruction.to_string().substr(29,3));

      myRF.ReadWrite(RegRs,RegRt,RegRd,0,0);
      opr1 = myRF.ReadData1;
      opr1 = myRF.ReadData2;

      ALUoutput = myALU.ALUOperation(Func,opr1,opr2);

    }
    //LOAD-TYPE

    else if(opcode==35){
      cout<<"LOAD-TYPE"<<"/n";
      bitset<5> RegRs(Instruction.to_string().substr(6,5));
      bitset<5> RegRt(Instruction.to_string().substr(11,5));

      myRF.ReadWrite(RegRs,0,0,0,0);
      opr1 = myRF.ReadData1;
      bitset<16> Imm(Instruction.to_string().substr(16,16));
      bitset<1> MSB(Imm.to_string().substr(0,1));

      if(MSB==0){
        opr2 = bitset<32>(0+Imm.to_ulong());
      }
      else if(MSB==1){
        opr2 = bitset<32>(1+Imm.to_ulong());
      }
      ALUoutput = myALU.ALUOperation(ADDU,opr1,opr2);
    }
    //STORE-TYPE

    else if(opcode==43){
      cout<<"STORE-TYPE"<<"/n";
      bitset<5> RegRs(Instruction.to_string().substr(6,5));
      bitset<5> RegRt(Instruction.to_string().substr(11,5));

      myRF.ReadWrite(RegRs,0,0,0,0);
      opr1 = myRF.ReadData1;
      bitset<16> Imm(Instruction.to_string().substr(16,16));
      bitset<1> MSB(Imm.to_string().substr(0,1));

      if(MSB==0){
        opr2 = bitset<32>(0+Imm.to_ulong());
      }
      else if(MSB==1){
        opr2 = bitset<32>(1+Imm.to_ulong());
      }
      ALUoutput = myALU.ALUOperation(ADDU,opr1,opr2);
    }
    else{
      cout<<"I-TYPE"<<"/n";
      bitset<5> RegRs(Instruction.to_string().substr(6,5));
      bitset<5> RegRt(Instruction.to_string().substr(11,5));
      bitset<16> Imm(Instruction.to_string().substr(16,16));
      bitset<1> MSB(Imm.to_string().substr(0,1));

      myRF.ReadWrite(RegRs,0,0,0,0);
      opr1 = myRF.ReadData1;

      if(MSB==0){
        opr2 = bitset<32>(0+Imm.to_ulong());
      }
      else if(MSB==1){
        opr2 = bitset<32>(1+Imm.to_ulong());
      }
      ALUoutput = myALU.ALUOperation(ADDU,opr1,opr2);

    }
    }
    //Stage 1
    cout<<"Stage 1"<<"/n";
    Instruction = myInsMem.ReadMemory(pc);

    pc = std::bitset<32> (pc.to_ulong()+4);

    // At the end of each cycle, fill in the corresponding data into "dumpResults" function to output files.
    // The first dumped pc value should be 0.
    dumpResults(pc,written_add_rf,written_output_rf,write_rf_en ,written_add_mem ,written_data_mem ,write_mem_en );
    }
	      myRF.OutputRF(); // dump RF;
          myDataMem.OutputDataMem(); // dump data mem

          return 0;

}
