cmd_/home/zixuan/drv_study/Module.symvers := sed 's/\.ko$$/\.o/' /home/zixuan/drv_study/modules.order | scripts/mod/modpost     -o /home/zixuan/drv_study/Module.symvers -e -i Module.symvers   -T -
