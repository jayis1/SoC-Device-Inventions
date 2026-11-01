#!/usr/bin/env python3
"""Emit a synthetic example waveform as CSV for plotting."""
import argparse, csv, math, sys
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--frequency',type=float,default=110.0); p.add_argument('--samples',type=int,default=800)
p.add_argument('--rate',type=float,default=8000.0); p.add_argument('--output',default='-')
a=p.parse_args()
if not (40<=a.frequency<=1500 and 8<=a.samples<=100000 and a.rate>0): p.error('frequency 40..1500 Hz, samples 8..100000, rate > 0 required')
out=sys.stdout if a.output=='-' else open(a.output,'w',newline='',encoding='utf-8')
try:
 w=csv.writer(out); w.writerow(['time_s','adc_code'])
 for i in range(a.samples): w.writerow([f'{i/a.rate:.6f}',round(2048+1200*math.sin(2*math.pi*a.frequency*i/a.rate))])
finally:
 if out is not sys.stdout: out.close()
