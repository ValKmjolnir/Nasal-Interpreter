# NasNeuron lib written by ValKmjolnir

# Basic Class NasMatrix
# NasMatGen  : generate a new matrix
# NasMatAdd  : add two matrixes
# NasMatSub  : sub two matrixes
# NasMatMul  : multiply two matrix
# NasMatTrans: transpose a matrix
# NasMatPrt  : print a matrix
var NasMatrix=
{
    NasMatGen:func(row,col)
    {
        var GenMat={Row:row,Col:col,Elem:[]};
        for(var i=0;i<row;i+=1)
        {
            var TmpVec=[];
            for(var j=0;j<col;j+=1)
                append(TmpVec,0);
            append(GenMat.Elem,TmpVec);
        }
        return GenMat;
    },
    NasMatAdd:func(mat1,mat2)
    {
        var ResultMat=nil;
        if(mat1.Row==mat2.Row and mat1.Col==mat2.Col)
        {
            ResultMat=me.NasMatGen(mat1.Row,mat1.Col);
            for(var i=0;i<ResultMat.Row;i+=1)
                for(var j=0;j<ResultMat.Col;j+=1)
                    ResultMat.Elem[i][j]=mat1.Elem[i][j]+mat2.Elem[i][j];
        }
        else
            print("NasNeuron: Mat1 and Mat2 have different rows and cols.");
        return ResultMat;
    },
    NasMatSub:func(mat1,mat2)
    {
        var ResultMat=nil;
        if(mat1.Row==mat2.Row and mat1.Col==mat2.Col)
        {
            ResultMat=me.NasMatGen(mat1.Row,mat1.Col);
            for(var i=0;i<ResultMat.Row;i+=1)
                for(var j=0;j<ResultMat.Col;j+=1)
                    ResultMat.Elem[i][j]=mat1.Elem[i][j]-mat2.Elem[i][j];
        }
        else
            print("NasNeuron: Mat1 and Mat2 have different rows and cols.");
        return ResultMat;
    },
    NasMatMul:func(mat1,mat2)
    {
        var ResultMat=nil;
        if(mat1.Col==mat2.Row)
        {
            ResultMat=me.NasMatGen(mat1.Row,mat2.Col);
            for(var i=0;i<ResultMat.Row;i+=1)
                for(var j=0;j<ResultMat.Col;j+=1)
                {
                    var sum=0;
                    for(var k=0;k<mat1.Col;k+=1)
                        sum+=mat1.Elem[i][k]*mat2.Elem[k][j];
                    ResultMat.Elem[i][j]=sum;
                }  
        }
        else
            print("NasNeuron: Mat1's Col is different from Mat2's Row.");
        return ResultMat;
    },
    NasMatTrans:func(mat)
    {
        var ResultMat=nil;
        ResultMat=me.NasMatGen(mat.Col,mat.Row);
        for(var i=0;i<ResultMat.Row;i+=1)
            for(var j=0;j<ResultMat.Col;j+=1)
                ResultMat.Elem[i][j]=mat.Elem[j][i];
        return ResultMat;
    },
    NasMatPrt:func(mat)
    {
        for(var i=0;i<mat.Row;i+=1)
        {
            for(var j=0;j<mat.Col;j+=1)
                print(mat.Elem[i][j],' ');
            print('\n');
        }
        return nil;
    }
};

NasMatrix.NasMatPrt(
    NasMatrix.NasMatGen(
        10,100
        )
    );