

declare namespace Biubiu
{
    export class CFoo
    {
        private constructor();
        public SetNumbers( fooNumbers: CFooNumbers ): void;
        public GetNumbers(): CFooNumbers;
        public Add( a: number, b: number ): void;
        public Sub( a: number, b: number ): void;
        public Hello( text: string ): void;

        public static new(): CFoo;
    }
}


declare class CFooNumbers
{
    private constructor();
    public Set( a: number, b: number ): void;
    public Print(): void;
    public BindCallback( callback: ( a: number, b: number ) => void ): void;

    public static new(): CFooNumbers;
}