function y = loggausspdf(X, mu, Sigma)
d = size(X,1);
X = bsxfun(@minus,X,mu);
[U,p]= chol(Sigma);
if p ~= 0
    error('ERROR: Sigma is not PD.');
end
disp('--');
Q = U'\X;disp(Q);disp('==');disp(U);
q = dot(Q,Q,1); % disp(q);% quadratic term (M distance)
c = d*log(2*pi)+2*sum(log(diag(U)));  % disp(c);% normalization constant
y = -(c+q)/2;

end