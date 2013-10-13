for OUTPUT in BrainHackDay1_Test1_Trial*
 do
	cat $output |awk '{print $1","$2","$3","$4","$5","$6","$6","$7","$8","$9","$10","$11","$12","$13","$14}' >  BrainHackDay1_Test1_Trials
done
cat 
