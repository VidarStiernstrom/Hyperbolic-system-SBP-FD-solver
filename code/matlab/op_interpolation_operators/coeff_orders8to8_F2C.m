function [stencil_F2C,BC_F2C,HcU,HfU] = coeff_orders8to8_F2C
stencil_F2C = [1.324608212425595e-3,-1.220703125e-3,-1.059686569940476e-2,1.1962890625e-2,3.708902994791667e-2,-5.9814453125e-2,-7.417805989583333e-2,2.99072265625e-1,5.927225748697917e-1,2.99072265625e-1,-7.417805989583333e-2,-5.9814453125e-2,3.708902994791667e-2,1.1962890625e-2,-1.059686569940476e-2,-1.220703125e-3,1.324608212425595e-3];
if nargout > 1
    BC_F2C = reshape([2.087365925359584,-1.227221822236823,1.090916714284468e1,-1.041312149906314,1.134214373258162,-1.269686811479259,2.075368250436277,-1.520771409696474e1,1.389750473974189,-1.48485748783569,-1.040579640776707,8.899721508624742e-1,-7.177691661032869,6.882712043721976e-1,-7.599347599660409e-1,-1.26440526850463,1.160658043364166,-5.391509178445742,6.679923135342851e-1,-7.521725463922262e-1,3.752349810676949,-2.906684049793639,2.672876913566556e1,-2.493145660873,2.782825829667436,1.443588084644871e-1,-1.307230271348211e-1,2.216804748506809,1.437719804483573e-1,-1.145830952112369e-1,3.2149320801083e-1,-2.34361439272989e-1,1.855837403850803,1.286992417665136e-1,-5.758238484341108e-2,-1.301103772185027,9.973408313121463e-1,-8.837520107531879,9.534930382604288e-1,-1.429199518808459e-2,-2.384453600787036,1.818345997558567,-1.577342441239303e1,1.422846302346762,3.154159322410927e-3,3.589377915408905e-1,-2.109524979864723e-1,9.363227187483732e-1,6.301238593470628e-2,5.732390257964316e-2,2.793582225299658,-2.033528056927806,1.621324028555783e1,-1.189222718426265,6.189671387692164e-2,5.342131492864642e-1,-4.222231296997605e-1,3.787253511209165,-3.350064226195165e-1,1.245325350855226e-2,-2.184146687275183,1.551331905163865,-1.19387312118926e1,8.349589004472998e-1,-1.35383591594438e-1,-8.014367493869704e-1,5.668415377001197e-1,-4.302842643863972,2.84183319528176e-1,3.83514596570461e-2,9.45794880458861e-1,-6.732638898386457e-1,5.234390273661413,-3.83498639699941e-1,1.440453495443018e-1,5.645065377027613e-1,-4.039220855868618e-1,3.170156487564899,-2.383074337879712e-1,1.216617375214008e-1,-1.937863224145611e-1,1.358891032584724e-1,-1.023378722711107,6.830621267493578e-2,-4.169135139842575e-3,2.270306426219975e-2,-2.38399209865252e-2,2.928388757260058e-1,-4.02060181933195e-2,6.880595419697505e-2,9.638828970166005e-2,-6.98778025332229e-2,5.609317796857676e-1,-4.429526993510467e-2,2.882554468024363e-2,3.582431391759518e-2,-2.671910644265286e-2,2.251335440088556e-1,-1.966486325944329e-2,1.791756577091828e-2,-3.350790201878844e-1,2.581411034605136e-1,-2.283079071439122,2.162064132948073e-1,-2.321676317817421e-1,6.356644993195555e-2,-4.921907141164279e-2,4.384534411523264e-1,-4.198474091936761e-2,4.597203884996652e-2,2.261662512481835e-2,-1.740429407604355e-2,1.537038474929879e-1,-1.452709057733876e-2,1.556101844926456e-2,3.097679325854209e-2,-2.394872918869654e-2,2.128879105995857e-1,-2.032077838507769e-2,2.213372706946953e-2],[5,24]);
end
if nargout > 2
    t2 = [2.948906761778786e-1,1.525720623897707,2.57452876984127e-1,1.798113701499118,4.127080577601411e-1,1.278484623015873,9.232955798059965e-1,1.009333860859158];
    HcU = t2;
end
if nargout > 3
    HfU = t2;
end
