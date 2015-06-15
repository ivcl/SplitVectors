function [R, llh] = expectation(X, model)
mu = model.mu;
disp(mu);

Sigma = model.Sigma;
disp(Sigma);
w = model.weight;
disp(w);

n = size(X,2);
k = size(mu,2);
logRho = zeros(n,k);

disp(logRho);
for i = 1:k
    logRho(:,i) = loggausspdf(X,mu(:,i),Sigma(:,:,i));
    disp(mu(:,i));
end

disp(logRho);

logRho = bsxfun(@plus,logRho,log(w));disp(logRho)
T = logsumexp(logRho,2);disp(T)
llh = sum(T)/n; disp(llh);% loglikelihood
logR = bsxfun(@minus,logRho,T);disp(logR)
R = exp(logR);disp(R)
end