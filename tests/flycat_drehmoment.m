% Martin Egli
% 2022-10-05
% measureing flycat 2204

clc, clf, clear all;

% measurements
% flycat 2204
l_mm = [55, 69];
m_g = [31, 22];

lm = l_mm .* m_g

l_m = l_mm./ 1000;
m_kg = m_g ./ 1000;
m_N = m_kg * 9.81;
Nm = l_m .* m_N

% Robot
Rl = 140; % mm
Rm = 182; % g

Rlm = Rl * Rm

ratio = Rlm ./ lm
max_alpha = asin(lm./Rlm).*180/pi
printf("the torque of this motor is sufficient to compensate approx. +/- 3° of robots falling angle, this is way too less");

figure(1);
plot(l_mm, m_g, '-x', 'linewidth', 3);
hold on;
plot(Rl, Rm, '-o', 'linewidth', 3);
hold off;
grid on;

