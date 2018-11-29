close all
clear all
clc

fileName = "scoretemp.txt";

stuff = load('stufftemp.txt');

y = load(fileName);

subplot(3,1,1)
plot(y)

subplot(3,1,2)
plot(y(end-30:end),'x-')

subplot(3,1,3)
plot(stuff)