

function [] = test(infileName,clusternum)

        clusternum = clusternum + 1;
        
        infile = fopen(infileName);

        num = fscanf(infile,'%d',1);
        num = 10;
        
        orientation = zeros(num,3);
        magnitude = zeros(num,1);
        position = zeros(num,3);

        display(num);
        points = zeros(num,9);
        for t = 1:num;         
            points(t,:)=fscanf(infile,'%f %f %f %f %f %f %f %f %f',9);
        end
        fclose(infile);

        % compute directions
        for t = 1:num;
            orientation(t,1) = points(t,6);
            orientation(t,2) = points(t,7);
            orientation(t,3) = points(t,8);
            magnitude(t,1) = points(t,9);
            position(t,1) = points(t,1);
            position(t,2) = points(t,2);
            position(t,3) = points(t,3);
            sum = points(t,6) * points(t,6) +points(t,7)* points(t,7) + points(t,8) * points(t,8);
            sum = sqrt(sum);
            orientation(t,1) = points(t,6)/sum;
            orientation(t,2) = points(t,7)/sum;
            orientation(t,3) = points(t,8)/sum;
        end
        
        data = [position(:,1),position(:,2),position(:,3),magnitude(:,1)]% orientation(:,1),orientation(:,2),orientation(:,3)];
        
        data2 = data';
        
        IDX = emgm(data2,clusternum);
        
        I = IDX';

        
        %spread(data,IDX)
        
        scatter3(data(:,1),data(:,2),data(:,3),50,I,'filled');
end





