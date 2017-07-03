function length = par_matmult(M)
% inputs: M = n x n matrix (1x1 struct)
% outputs: elapsed time and dimension of matrix (n x n)

mat = full(M.Problem.A);
length = size(mat,1);

tic
B = zeros(length);
parfor j=1:length
  for i=1:length
      for k=1:length
      B(i,j) = B(i,j) + mat(i,k)*mat(k,j);
      end
  end
end
toc;

disp(length);

%delete(gcp('nocreate'));

end

