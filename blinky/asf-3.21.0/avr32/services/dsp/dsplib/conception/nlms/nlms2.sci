function [e, y, w]  = nlms(u, d, a, b, w0)

% [e, y, w] = nlms(u, d, a, b, w0) �ϡ�
% NLMS (normalized least mean square)ˡ���Ѥ���1����FIRŬ���ե��륿��¸����롥
%
%   u  : ���Ͽ��� (1xK)
%   d  : ����(��Ū)���� (1xK)
%   a  : �ѥ�᡼�� (���μ¿�)
%   b  : �ѥ�᡼�� (���μ¿�)
%   w0 : �ե��륿�����٥��ȥ�ν���� (Lx1)
%   e  : ������ (1xK)
%   y  : ���Ͽ��� (1xK)
%   w  : �ե��륿�����٥��ȥ�λ����� (LxK)
%
%   NLMS ���르�ꥺ�� : wnew = w + 2 * a * x * e / (x'*x + b)
%
% 2000.12.01  OHKI Makoto

K = length(u);
L = length(w0);
u = [zeros(1, L-1), u];
e = zeros(1, K);
y = zeros(1, K);
w = zeros(L, K+1);
w(:,1) = w0;
for k=1:K
  x = u(k+L-1:-1:k)';
  y(k) = x' * w(:, k);
  e(k) = d(k) - y(k);
  w(:, k+1) = w(:, k) + 2 * a * x * e(k) / (x' * x + b);
end
w = w(:, 1:K);
