function directLegend(outfileName)
    % calculate
    numLati = 19;
    numLongi = 36;
    points = zeros(numLati*numLongi,3);
    idx = 0;
    colors = zeros(numLati*numLongi,3);
    for i=1:numLongi;
        longi1 = (i-1)*10/180*pi();
        for j=1:numLati;
            idx = idx+1;
            lati = ((j-1)*10-90)/180*pi();
            points(idx,1)=cos(longi1)*cos(lati);
            points(idx,2)=sin(longi1)*cos(lati);
            points(idx,3)=sin(lati);
            
            shiftNormPoint = (points(idx,:)/2+0.5);
            colors(idx,:)=shiftNormPoint/norm(shiftNormPoint);
        end
    end
    
    % write
    outfile = fopen(outfileName,'w');
    fprintf(outfile,'%u %u\n',numLongi,numLati);
    for t = 1:numLati*numLongi;
        fprintf(outfile,'%f %f %f',points(t,1),points(t,2),points(t,3));
        fprintf(outfile,' %f %f %f \n',colors(t,1),colors(t,2),colors(t,3));
    end
    fclose(outfile);
end