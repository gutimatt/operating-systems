cmd_/home/mrgutie8/CLionProjects/LKM/Module.symvers := sed 's/\.ko$$/\.o/' /home/mrgutie8/CLionProjects/LKM/modules.order | scripts/mod/modpost -m -a  -o /home/mrgutie8/CLionProjects/LKM/Module.symvers -e -i Module.symvers   -T -